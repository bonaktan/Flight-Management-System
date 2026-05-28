-- Add Airport (ADMIN)
INSERT INTO airport (name, id, capacity) VALUES (
	'Ninoy Aquino International Airport',
	'MNL',
	61
);

-- Add Airplane (ADMIN)
INSERT INTO airplane (id, model, location) VALUES (
	'SB-M0067',
	'Airbus A320',
	'MNL'
);


-- Add Seat Class (ADMIN)
INSERT INTO seat_class (class_name, amt_of_seats, markup_price, airplane_id) VALUES (
	'Economy',
	144,
	1,
	'SB-M0067'
) RETURNING id;

