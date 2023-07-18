
#tempos="-t1s -t20s -t60s"
#usuarios="-c20 -c100 -c255 -c1000"

ip="http://127.0.0.1:40000"

for s in $(cat parametros/s.txt);
do
	echo "sleep 15"
	serv="-"$s
	echo "./servidor.out $serv &"
	for t in $(cat parametros/t.txt);
	do
		for c in $(cat parametros/c.txt);
		do
			echo siege $c $t $ip -l"logs/"$s"-"$c"-"$t".log"
			#echo siege $c $t $ip -l$s"-"$c"-"$t".txt"
		done
	done
	echo "pkill servidor.out"
done