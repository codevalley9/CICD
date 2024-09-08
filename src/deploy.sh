#!/bin/bash

ssh lysonowi@172.24.116.8 "rm /usr/local/bin/s21_cat"
ssh lysonowi@172.24.116.8 "rm /usr/local/bin/s21_grep"
scp ./src/cat/s21_cat lysonowi@172.24.116.8:/usr/local/bin/
scp ./src/grep/s21_grep lysonowi@172.24.116.8:/usr/local/bin/

