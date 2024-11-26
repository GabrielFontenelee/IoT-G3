#Script for sending coordinates every 0.5 seconds to the mosquitto server

while true
do
    # random coordinates
    x=$(shuf -i 240-260 -n 1)
    y=$(shuf -i 240-260 -n 1)
    z=$(shuf -i 0-100 -n 1)
    Z=$((z-50))
    theta=$(shuf -i 0-360 -n 1)
    mosquitto_pub -t "test/coordinates" -m "$x,$y,$Z,$theta"
    sleep 0.1
done