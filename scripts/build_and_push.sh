#!/bin/bash

set -euxo pipefail

docker build --tag dmahon10/adversarial_chair:latest .
docker push dmahon10/adversarial_chair:latest