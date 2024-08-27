SELECT m.title, r.rating FROM movies AS m, ratings AS r
WHERE m.id = r.movie_id AND m.year = 2010 ORDER BY r.rating DESC, m.title;
