#!/bin/sh

jq '.db_clients[0] |= (
    .host = (env.POSTGRES_HOST // .host) |
    .port = (if (env.POSTGRES_PORT and env.POSTGRES_PORT != "") then (env.POSTGRES_PORT | tonumber) else .port end)  | 
    .passwd = (env.POSTGRES_PASSWORD // .passwd) | 
    .dbname = (env.POSTGRES_DB // .dbname) | 
    .user = (env.POSTGRES_USER // .user)
)' /app/config.json > /app/config.tmp 

mv /app/config.tmp /app/config.json
exec /app/SkyBridgeAirways-Backend
