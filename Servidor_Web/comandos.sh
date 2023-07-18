sleep 15
./servidor.out -s1 &
siege -c20 -t1s http://127.0.0.1:40000 -llogs/s1--c20--t1s.log
siege -c100 -t1s http://127.0.0.1:40000 -llogs/s1--c100--t1s.log
siege -c255 -t1s http://127.0.0.1:40000 -llogs/s1--c255--t1s.log
siege -c750 -t1s http://127.0.0.1:40000 -llogs/s1--c750--t1s.log
siege -c20 -t20s http://127.0.0.1:40000 -llogs/s1--c20--t20s.log
siege -c100 -t20s http://127.0.0.1:40000 -llogs/s1--c100--t20s.log
siege -c255 -t20s http://127.0.0.1:40000 -llogs/s1--c255--t20s.log
siege -c750 -t20s http://127.0.0.1:40000 -llogs/s1--c750--t20s.log
siege -c20 -t60s http://127.0.0.1:40000 -llogs/s1--c20--t60s.log
siege -c100 -t60s http://127.0.0.1:40000 -llogs/s1--c100--t60s.log
siege -c255 -t60s http://127.0.0.1:40000 -llogs/s1--c255--t60s.log
siege -c750 -t60s http://127.0.0.1:40000 -llogs/s1--c750--t60s.log
pkill servidor.out
sleep 15
./servidor.out -s2 &
siege -c20 -t1s http://127.0.0.1:40000 -llogs/s2--c20--t1s.log
siege -c100 -t1s http://127.0.0.1:40000 -llogs/s2--c100--t1s.log
siege -c255 -t1s http://127.0.0.1:40000 -llogs/s2--c255--t1s.log
siege -c750 -t1s http://127.0.0.1:40000 -llogs/s2--c750--t1s.log
siege -c20 -t20s http://127.0.0.1:40000 -llogs/s2--c20--t20s.log
siege -c100 -t20s http://127.0.0.1:40000 -llogs/s2--c100--t20s.log
siege -c255 -t20s http://127.0.0.1:40000 -llogs/s2--c255--t20s.log
siege -c750 -t20s http://127.0.0.1:40000 -llogs/s2--c750--t20s.log
siege -c20 -t60s http://127.0.0.1:40000 -llogs/s2--c20--t60s.log
siege -c100 -t60s http://127.0.0.1:40000 -llogs/s2--c100--t60s.log
siege -c255 -t60s http://127.0.0.1:40000 -llogs/s2--c255--t60s.log
siege -c750 -t60s http://127.0.0.1:40000 -llogs/s2--c750--t60s.log
pkill servidor.out
sleep 15
./servidor.out -s3 &
siege -c20 -t1s http://127.0.0.1:40000 -llogs/s3--c20--t1s.log
siege -c100 -t1s http://127.0.0.1:40000 -llogs/s3--c100--t1s.log
siege -c255 -t1s http://127.0.0.1:40000 -llogs/s3--c255--t1s.log
siege -c750 -t1s http://127.0.0.1:40000 -llogs/s3--c750--t1s.log
siege -c20 -t20s http://127.0.0.1:40000 -llogs/s3--c20--t20s.log
siege -c100 -t20s http://127.0.0.1:40000 -llogs/s3--c100--t20s.log
siege -c255 -t20s http://127.0.0.1:40000 -llogs/s3--c255--t20s.log
siege -c750 -t20s http://127.0.0.1:40000 -llogs/s3--c750--t20s.log
siege -c20 -t60s http://127.0.0.1:40000 -llogs/s3--c20--t60s.log
siege -c100 -t60s http://127.0.0.1:40000 -llogs/s3--c100--t60s.log
siege -c255 -t60s http://127.0.0.1:40000 -llogs/s3--c255--t60s.log
siege -c750 -t60s http://127.0.0.1:40000 -llogs/s3--c750--t60s.log
pkill servidor.out
sleep 15
./servidor.out -s4 &
siege -c20 -t1s http://127.0.0.1:40000 -llogs/s4--c20--t1s.log
siege -c100 -t1s http://127.0.0.1:40000 -llogs/s4--c100--t1s.log
siege -c255 -t1s http://127.0.0.1:40000 -llogs/s4--c255--t1s.log
siege -c750 -t1s http://127.0.0.1:40000 -llogs/s4--c750--t1s.log
siege -c20 -t20s http://127.0.0.1:40000 -llogs/s4--c20--t20s.log
siege -c100 -t20s http://127.0.0.1:40000 -llogs/s4--c100--t20s.log
siege -c255 -t20s http://127.0.0.1:40000 -llogs/s4--c255--t20s.log
siege -c750 -t20s http://127.0.0.1:40000 -llogs/s4--c750--t20s.log
siege -c20 -t60s http://127.0.0.1:40000 -llogs/s4--c20--t60s.log
siege -c100 -t60s http://127.0.0.1:40000 -llogs/s4--c100--t60s.log
siege -c255 -t60s http://127.0.0.1:40000 -llogs/s4--c255--t60s.log
siege -c750 -t60s http://127.0.0.1:40000 -llogs/s4--c750--t60s.log
pkill servidor.out

