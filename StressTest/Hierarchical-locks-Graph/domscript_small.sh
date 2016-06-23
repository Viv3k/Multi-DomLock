echo >./output/DomLockG.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 5 $((2**$j)) $((2**$i)) s >>./output/DomLockG.txt;
done
echo >>./output/DomLockG.txt
done

