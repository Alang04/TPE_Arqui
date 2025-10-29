docker start TPE_Arqui_2Q_2025

docker exec -it TPE_Arqui_2Q_2025 make -C /root/Toolchain clean
docker exec -it TPE_Arqui_2Q_2025 make -C /root/Toolchain all
docker exec -it TPE_Arqui_2Q_2025 make -C /root clean
docker exec -it TPE_Arqui_2Q_2025 make -C /root all