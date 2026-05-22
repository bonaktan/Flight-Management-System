#!/bin/sh
jq '.db_clients[0] |= (.host = env.POSTGRES_HOST | .port = (env.POSTGRES_PORT | tonumber) | .passwd = env.POSTGRES_PASSWORD | .dbname = env.POSTGRES_DB | .user = env.POSTGRES_USER)' /app/config.json > /app/config.tmp 
mv /app/config.tmp /app/config.json
/app/SkyBridgeAirways-Backend