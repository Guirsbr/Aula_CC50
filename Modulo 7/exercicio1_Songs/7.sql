SELECT AVG(energy) FROM songs WHERE artist_id IN 
(SELECT id FROM artists WHERE name = 'Drake');

-- Slower second option:
-- SELECT AVG(s.energy) FROM songs AS s, artists AS a WHERE s.artist_id = a.id AND a.name = 'Drake';
