clear && clear

n_nodes=11
n_eval=21

mkdir -p data_graph

for set_number in 1 2 3 4; do
    ./task_8 ${set_number} ${n_nodes} ${n_eval} > data_graph/set_${set_number}_n${n_nodes}.txt
    echo "Saved data_graph/set_${set_number}_n${n_nodes}.txt"
done
