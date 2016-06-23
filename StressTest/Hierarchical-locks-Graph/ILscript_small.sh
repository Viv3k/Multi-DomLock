echo >./output/Intention.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 4 $((2**$j)) $((2**$i)) s >>./output/Intention.txt
done
echo >>./output/Intention.txt
done

echo >>./output/Intention.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 4 $((2**$j)) $((2**$i)) m >>./output/Intention.txt
done
echo >>./output/Intention.txt
done

echo >>./output/Intention.txt
for j in {1..6};do
for i in {0..4};do
./a.out 1000 2 4 $((2**$j)) $((2**$i)) l >>./output/Intention.txt
done
echo >>./output/Intention.txt
done

