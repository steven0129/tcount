for i in 1 2 5 10 20 17 33 256 512 
do sudo perf record -o perf.data$i ./a.out -hv $i sents.rec > result$i.txt
done
