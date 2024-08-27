SELECT s.name FROM songs AS s, artists AS a WHERE s.artist_id = a.id AND a.name = 'Post Malone';

-- Slower second option:
-- SELECT name FROM songs WHERE artist_id IN (SELECT id FROM artists WHERE name = 'Post Malone');
