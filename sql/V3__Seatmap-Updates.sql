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
    


DROP TABLE seat_class;