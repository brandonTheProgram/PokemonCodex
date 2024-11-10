#!/bin/bash

EXECUTABLE_NAME="PokemonCodex.exe"
EXPRESS_SERVER="node.exe"

echo "Stopping all servers..."

# Kill the backend and Express server
wmic process where "name='$EXECUTABLE_NAME'" delete
wmic process where "name='$EXPRESS_SERVER'" delete

echo "Servers stopped."