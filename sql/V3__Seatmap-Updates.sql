ALTER TABLE IF EXISTS public.airplane
    ADD COLUMN seatmap json NOT NULL DEFAULT '{}';

ALTER TABLE IF EXISTS public.airplane
    ADD COLUMN seat_class json NOT NULL DEFAULT '{}'; 

ALTER TABLE IF EXISTS public.booking
    ADD COLUMN seat_id character varying(4) NOT NULL;
DROP TABLE seat_class;