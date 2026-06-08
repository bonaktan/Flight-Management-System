ALTER TABLE IF EXISTS public.airplane
    ADD COLUMN seatmap json NOT NULL DEFAULT '{}';

ALTER TABLE IF EXISTS public.airplane
    ADD COLUMN seat_class json NOT NULL DEFAULT '{}'; 

ALTER TABLE IF EXISTS public.booking
    ADD COLUMN seat_id character varying(4) NOT NULL;

ALTER TABLE IF EXISTS public.passenger
    ADD COLUMN middle_name TEXT NOT NULL,
    ADD COLUMN gender TEXT NOT NULL,
    ADD COLUMN phone_number TEXT NOT NULL,
    ADD COLUMN emergency_contact_phone TEXT NOT NULL;

ALTER TABLE booking 
    ADD CONSTRAINT unique_seat_per_flight_date 
    UNIQUE (flight_id, seat_id, departure_date);

CREATE TABLE booking_passenger (
    booking_id BIGINT NOT NULL REFERENCES booking(id),
    passenger_id BIGINT NOT NULL REFERENCES passenger(id),
    seat_id VARCHAR(4) NOT NULL,
    calculated_price NUMERIC(10, 2) NOT NULL CHECK (calculated_price >= 0),
    PRIMARY KEY (booking_id, passenger_id)
);
ALTER TABLE booking
    DROP COLUMN passenger_id,
    DROP COLUMN seat_id,
    DROP COLUMN calculated_price;
DROP TABLE seat_class;