PROG=$1
for i in $(seq 0 9); do
    echo "[TOY $i]"
    $PROG < $2'/in/toy_'$i'_input' | tee '/tmp/toy_'$i'_output';
    git diff $2'/out/toy_'$i'/index' $2'/outref/toy_'$i'/index'
done
