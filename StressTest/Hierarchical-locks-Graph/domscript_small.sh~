echo >./output/DomLock.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 5 $((2**$j)) $((2**$i)) s >>./output/DomLock.txt;
done
echo >>./output/DomLock.txt
done

echo >>./output/DomLock.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 5 $((2**$j)) $((2**$i)) m >>./output/DomLock.txt;
done
echo >>./output/DomLock.txt
done

echo >>./output/DomLock.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 5 $((2**$j)) $((2**$i)) l >>./output/DomLock.txt;
done
echo >>./output/DomLock.txt
done

