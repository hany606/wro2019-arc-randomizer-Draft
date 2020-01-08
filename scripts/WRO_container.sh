#!/bin/bash
docker build -t wro_img ..
docker run -it --name WRO --mount type=bind,src="$(pwd)",target=/home/shared wro_img bash

