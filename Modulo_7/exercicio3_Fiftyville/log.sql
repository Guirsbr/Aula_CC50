-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Procurar a descriçaõ do crime.
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2023 AND street = 'Humphrey Street';
-- Foi descoberto que tiveram 3 testemunhas e o roubo foi em uma padaria.
-- O roubo foi as 10:15am

-- Saber mais Sobre o relato das 3 testemunhas.
SELECT name, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2023;
-- O ladrão fugiu em um carro 10 minutos após o roubo pelo estacionamento da padaria.
-- A padaria é da Emma.
-- O ladrão estava retirando dinheiro de um ATM antes de ir para o roubo na Leggett Street.
-- O ladrão planejava pegar o voo mais cedo do próximo dia e pediu para um cumplice em ligação que durou menos de 1 minuto ao sair da padaria para comprar as passagens.

-- Identificar possiveis suspeitos.
SELECT name, passport_number, license_plate FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2023 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'));
-- +---------+-----------------+---------------+
-- |  name   | passport_number | license_plate |
-- +---------+-----------------+---------------+
-- | Kenny   | 9878712108      | 30G67EN       |
-- | Iman    | 7049073643      | L93JTIZ       |
-- | Benista | 9586786673      | 8X428L0       |
-- | Taylor  | 1988161715      | 1106N58       |
-- | Brooke  | 4408372428      | QX4YZN3       |
-- | Luca    | 8496433585      | 4328GD8       |
-- | Diana   | 3592750733      | 322W7JE       |
-- | Bruce   | 5773159633      | 94KL13X       |
-- +---------+-----------------+---------------+

-- Identificar a placa do ladrão de acordo com o testemunho de uma das testemunhas.
SELECT hour, minute, license_plate FROM bakery_security_logs WHERE activity = 'exit' AND day = 28 AND month = 7 AND year = 2023 AND (hour = 10 OR hour = 11);
-- Pricipais suspeitos: Diana, Iman.   Outros possiveis suspeitos: Luca, Taylor, Bruce.

-- Inspecionar a ligação relatada por uma das testemunhas.
SELECT cal_name, rec_name, duration FROM
(((SELECT name AS cal_name, phone_number AS cal_phone_number FROM people)
INNER JOIN
(SELECT caller, id, duration FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2023 AND duration <= 60)
ON cal_phone_number = caller)
INNER JOIN
((SELECT name AS rec_name, phone_number AS rec_phone_number FROM people)
INNER JOIN
(SELECT receiver, id FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2023 AND duration <= 60)
ON rec_phone_number = receiver)
USING (id));
-- Pricipais suspeitos: Diana como Ladra, Philip como Cúmplice.   Outros possiveis suspeitos: Bruce como Ladrão, Robin como Cúmplice.

-- Investigar o voo relatado por uma das testemunhas.
SELECT cal_name, rec_name FROM
SELECT passanger_name, destination_city, hour, minute FROM
(((((SELECT id AS flight_id, origin_airport_id, destination_airport_id, hour, minute FROM flights WHERE day = 29 AND month = 7 AND year = 2023) AS f
INNER JOIN
(SELECT id, city AS origin_city FROM airports where city = 'Fiftyville') AS a1
ON f.origin_airport_id = a1.id) AS f
INNER JOIN
(SELECT id, city AS destination_city FROM airports) AS a2
ON f.destination_airport_id = a2.id)
INNER JOIN
(SELECT flight_id, passport_number FROM passengers)
USING (flight_id))
INNER JOIN
(SELECT name AS passanger_name, passport_number FROM people WHERE name = 'Diana' OR name = 'Bruce')
USING (passport_number));
-- Após analisar todas as provas novamente o me veredito é:
-- Ladrão: Bruce
-- Cidade que escapou: New York City
-- Cúmplice: Robin
