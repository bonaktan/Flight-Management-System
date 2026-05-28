START TRANSACTION;

-- Core entities
CREATE TABLE account (
    id BIGSERIAL PRIMARY KEY,
    account_name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    permissions JSONB DEFAULT '{}',
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE TABLE airport (
    name TEXT NOT NULL,
    id TEXT UNIQUE NOT NULL PRIMARY KEY,
    capacity INTEGER CHECK (capacity > 0),
    created_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE TABLE staff (
    id BIGSERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    current_location TEXT NOT NULL,
    role TEXT NOT NULL,
    schedule JSONB,
    created_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE TABLE passenger (
    id BIGSERIAL PRIMARY KEY,
    frequent_flyer_code TEXT UNIQUE,
    title TEXT NOT NULL,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    birthdate DATE NOT NULL,
    contact_email TEXT NOT NULL,
    emergency_contact_name TEXT NOT NULL,
    emergency_contact_email TEXT NOT NULL,
    associated_to BIGINT NOT NULL REFERENCES account(id),
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE TABLE flight (
    id TEXT NOT NULL PRIMARY KEY,
    departure_airport_id TEXT NOT NULL REFERENCES airport(id),
    arrival_airport_id TEXT NOT NULL REFERENCES airport(id),
    base_ticket_price NUMERIC(10, 2) NOT NULL,
    flight_time INTERVAL NOT NULL,
    departure TIMESTAMPTZ NOT NULL,
    frequency INTERVAL NOT NULL,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    CHECK (departure_airport_id != arrival_airport_id)
);
CREATE TABLE airplane (
    id TEXT NOT NULL PRIMARY KEY,
    model TEXT NOT NULL,
    location TEXT REFERENCES airport(id)
);
CREATE TABLE seat_class (
    id BIGSERIAL PRIMARY KEY,
    class_name TEXT NOT NULL,
    amt_of_seats INTEGER NOT NULL CHECK (amt_of_seats > 0),
    markup_price NUMERIC(10, 2) NOT NULL,
    airplane_id TEXT NOT NULL REFERENCES airplane(id),
    created_at TIMESTAMPTZ DEFAULT NOW()
);

-- Junction tables
CREATE TABLE flight_staff (
    flight_id TEXT REFERENCES flight(id) ON DELETE CASCADE,
    staff_id BIGINT REFERENCES staff(id) ON DELETE CASCADE,
    PRIMARY KEY (flight_id, staff_id)
);
CREATE TABLE booking (
    id BIGSERIAL PRIMARY KEY,
    passenger_id BIGINT NOT NULL REFERENCES passenger(id),
    flight_id TEXT NOT NULL REFERENCES flight(id),
    account_id BIGINT REFERENCES account(id),
    calculated_price NUMERIC(10, 2) NOT NULL,
    payment_option TEXT NOT NULL,
    payment_detail JSONB NOT NULL,
    booking_status TEXT DEFAULT 'pending',
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);
CREATE TABLE airport_flight (
    airport_id TEXT REFERENCES airport(id) ON DELETE CASCADE,
    flight_id TEXT REFERENCES flight(id) ON DELETE CASCADE,
    relationship_type TEXT NOT NULL,
    PRIMARY KEY (airport_id, flight_id, relationship_type)
);

-- Indexes
CREATE INDEX idx_passengers_email ON passenger(contact_email);
CREATE INDEX idx_bookings_passenger ON booking(passenger_id);
CREATE INDEX idx_bookings_flight ON booking(flight_id);
CREATE INDEX idx_flight_departure ON flight(departure_airport_id);
CREATE INDEX idx_flight_arrival ON flight(arrival_airport_id);
CREATE INDEX idx_flight_date ON flight(departure);

COMMIT