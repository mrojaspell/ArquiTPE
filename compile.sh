docker run -d -v /mnt/c/Users/Agustin\ Z/Documents\GitHub\ArquiTPE:/root --security-opt seccomp:unconfined -ti --name arqui agodio/itba-so:1.0
docker exec -it arqui make clean    -C /root/Toolchain
docker exec -it arqui make all      -C /root/Toolchain
docker exec -it arqui make clean    -C /root/
docker exec -it arqui make all      -C /root/
docker stop arqui
docker rm arqui