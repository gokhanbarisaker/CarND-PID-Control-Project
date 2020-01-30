#!/usr/bin/env bash

docker build --no-cache --rm -f "Project.dockerfile" -t carnd-pid-project:latest . && docker run -p 4567:4567 --rm -it carnd-pid-project:latest
