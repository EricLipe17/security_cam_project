#!/bin/bash

sudo apt-get install postgresql
sudo apt-get install libpq-dev
sudo service postgresql start
sudo -u postgres createuser singularity
sudo -u postgres psql postgres -c "ALTER USER singularity WITH encrypted password '!singularityuser!';"
sudo -u postgres createdb singularity --owner singularity
