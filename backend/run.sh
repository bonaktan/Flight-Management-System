#!/bin/sh

jq '.db_clients[0] |= (
    .host = (env.DATABASE_HOST // .host) |
    .port = (if (env.DATABASE_PORT and env.DATABASE_PORT != "") then (env.DATABASE_PORT | tonumber) else .port end)  | 
    .passwd = (env.DATABASE_PASSWORD // .passwd) | 
    .dbname = (env.DATABASE_DB // .dbname) | 
    .user = (env.DATABASE_USER // .user)
)' /app/config.json > /app/config.tmp 

mv /app/config.tmp /app/config.json
exec /app/SkyBridgeAirways-Backend
