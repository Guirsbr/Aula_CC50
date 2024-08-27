SELECT m.title FROM movies AS m, ratings AS a WHERE m.id = a.movie_id AND m.id IN
(SELECT movie_id FROM stars WHERE person_id IN
(SELECT id FROM people WHERE name = 'Chadwick Boseman'))
ORDER BY a.rating DESC LIMIT 5;
