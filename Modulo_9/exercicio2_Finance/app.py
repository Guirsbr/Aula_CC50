import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user Database
    user_db = db.execute("SELECT id, cash FROM users WHERE id = ?", session["user_id"])[0]

    # Get user Wallets
    wallets_db = db.execute(
        "SELECT w.quantity AS quantity, s.symbol AS symbol FROM users AS u, wallets AS w, stocks AS s WHERE u.id = w.idUsers AND w.idStocks = s.id AND u.id = ?", session["user_id"]
        )

    # Get all stocks prices
    for index in range(len(wallets_db)):
        stock_quote = lookup(wallets_db[index]["symbol"])
        wallets_db[index]["price"] = stock_quote["price"]
        wallets_db[index]["total_price"] = wallets_db[index]["price"] * wallets_db[index]["quantity"]

    # Get the total cash of the user, including stocks owned br the user
    user_db["total_cash"] = user_db["cash"]
    for index in range(len(wallets_db)):
        user_db["total_cash"] += wallets_db[index]["price"] * wallets_db[index]["quantity"]

    # Convert numbers to US money
    for index in range(len(wallets_db)):
        wallets_db[index]["price"] = usd(wallets_db[index]["price"])
        wallets_db[index]["total_price"] = usd(wallets_db[index]["total_price"])
    user_db["total_cash"] = usd(user_db["total_cash"])
    user_db["cash"] = usd(user_db["cash"])

    # Send stocks and user informations to the view
    return render_template("index.html", wallets_db=wallets_db, user_db=user_db)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return apology("must provide stock’s symbol", 403)

        # Ensure number of shares was submitted
        elif not request.form.get("num_shares"):
            return apology("must provide number of shares", 403)

        # Validate the number of shares input
        try:
            num_shares = int(request.form.get("num_shares"))
            if num_shares <= 0:
                return apology("must provide an natural number of shares", 403)
        except ValueError:
            return apology("must provide an natural number of shares", 403)

        # Try to get stock quote from API
        stock_symbol = request.form.get("stock_symbol").upper()
        stock_quote = lookup(stock_symbol)
        if not stock_quote:
            return apology("the quote provided was not founded", 403)

        # Set the amount user have to pay
        stocks_price = stock_quote["price"] * num_shares

        # Get user Database
        user_db = db.execute("SELECT id, cash FROM users WHERE id = ?", session["user_id"])[0]

        # Verify if user have enough money
        if stocks_price > user_db["cash"]:
            return apology(f"you are short {usd(stocks_price - user_db["cash"])} to be able to buy these shares", 403)

        # Get stock Database, if not, register the stock on Database
        stock_db = db.execute("SELECT id FROM stocks WHERE symbol = ?", stock_symbol)
        if not stock_db:
            db.execute("INSERT INTO stocks (symbol) VALUES (?)", stock_symbol)
            stock_db = db.execute("SELECT id FROM stocks WHERE symbol = ?", stock_symbol)[0]
        else:
            stock_db = stock_db[0]

        # Update new user money
        user_db["cash"] -= stocks_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_db["cash"], user_db["id"])

        # Add new transaction to Database
        today = datetime.now()
        current_dateTime = today.strftime('%d/%m/%Y-%H:%M')
        db.execute(
            "INSERT INTO transactions (idStocks, idUsers, dateTime, type, quantity, price) VALUES (?, ?, ?, ?, ?, ?)", stock_db["id"], user_db["id"], current_dateTime, "BUY", num_shares, stock_quote["price"]
            )

        # Get users Wallet Database
        wallet_db = db.execute("SELECT quantity FROM wallets WHERE idStocks = ? AND idUsers = ?", stock_db["id"], user_db["id"])

        # Add the stocks bought to user's wallet
        if not wallet_db:
            db.execute("INSERT INTO wallets VALUES (?, ?, ?)", stock_db["id"], user_db["id"], num_shares)
        else:
            db.execute("UPDATE wallets SET quantity = ? WHERE idStocks = ? AND idUsers = ?",wallet_db[0]["quantity"] + num_shares, stock_db["id"], user_db["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get transactions from the user
    transactions_db = db.execute(
        "SELECT t.dateTime AS dateTime, t.type AS type, t.quantity AS quantity, t.price AS price, s.symbol AS symbol FROM users AS u, transactions AS t, stocks AS s WHERE u.id = t.idUsers AND t.idStocks = s.id AND u.id = ? ORDER BY t.id DESC", session["user_id"]
        )

    for index in range(len(transactions_db)):
        # Convert int to US money string
        transactions_db[index]["total_price"] = usd(transactions_db[index]["price"] * transactions_db[index]["quantity"])
        transactions_db[index]["price"] = usd(transactions_db[index]["price"])

        # Separate Date and Time
        transactions_db[index]["dateTime"] = transactions_db[index]["dateTime"].split("-")

    # Send trsnsactions informations to the view
    return render_template("history.html", transactions_db=transactions_db)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return redirect("/quote")

        # Try to get stock quote from API
        stock_quote = lookup(request.form.get("stock_symbol"))
        if not stock_quote:
            return apology("the quote provided was not founded", 403)

        # Send stocks information to the view
        stock_quote["price"] = usd(stock_quote["price"])
        return render_template("quote.html", quoted=True, stock_quote=stock_quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html", quoted=False)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Try to register on database
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password"))
            )

        except ValueError:
            return apology("username already exist, please try another one", 403)

        # Redirect to login page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return apology("must provide stock’s symbol", 403)

        # Ensure number of shares was submitted
        elif not request.form.get("num_shares"):
            return apology("must provide number of shares", 403)

        # Validate the number of shares input
        try:
            num_shares = int(request.form.get("num_shares"))
            if num_shares <= 0:
                return apology("must provide an natural number of shares", 403)
        except ValueError:
            return apology("must provide an natural number of shares", 403)

        # Try to get stock quote from API
        stock_symbol = request.form.get("stock_symbol").upper()
        stock_quote = lookup(stock_symbol)
        if not stock_quote:
            return apology("the quote provided was not founded", 403)

        # Set the amount of money user will get
        stocks_price = stock_quote["price"] * num_shares

        # Get user Database
        user_db = db.execute("SELECT id, cash FROM users WHERE id = ?", session["user_id"])[0]

        # Try to get Wallet Database for that stock
        wallet_db = db.execute(
            "SELECT w.quantity AS quantity, w.idStocks AS idStocks FROM wallets AS w, stocks AS s WHERE w.idStocks = s.id AND s.symbol = ? AND w.idUsers = ?", stock_symbol, user_db["id"]
            )
        if not wallet_db:
            return apology("hey! stop snitchin around with the html", 403)
        wallet_db = wallet_db[0]

        # Verify if user have enough shares os that stock
        if wallet_db["quantity"] < num_shares:
            return apology("you don't have the enough shares that you want to sell", 403)

        # Update new user money
        user_db["cash"] += stocks_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_db["cash"], user_db["id"])

        # Add new transaction to Database
        today = datetime.now()
        current_dateTime = today.strftime('%d/%m/%Y-%H:%M')
        db.execute(
            "INSERT INTO transactions (idStocks, idUsers, dateTime, type, quantity, price) VALUES (?, ?, ?, ?, ?, ?)", wallet_db["idStocks"], user_db["id"], current_dateTime, "SELL", num_shares, stock_quote["price"]
            )

        # Remove the stocks sold from user's wallet
        if wallet_db["quantity"] == num_shares:
            db.execute("DELETE FROM wallets WHERE idStocks = ? AND idUsers = ?", wallet_db["idStocks"], user_db["id"])
        else:
            db.execute("UPDATE wallets SET quantity = ? WHERE idStocks = ? AND idUsers = ?", wallet_db["quantity"] - num_shares, wallet_db["idStocks"], user_db["id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get wallets from user
        wallets_db = db.execute(
            "SELECT s.symbol AS symbol FROM users AS u, wallets AS w, stocks AS s WHERE u.id = w.idUsers AND w.idStocks = s.id AND u.id = ?", session["user_id"]
            )

        # Send symbols from wallet's user to view
        return render_template("sell.html", wallets_db=wallets_db)
