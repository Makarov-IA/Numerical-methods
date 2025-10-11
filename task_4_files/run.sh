clear&&clear

for number_of_schema in {1..6}; do
    ./task_4 ${number_of_schema}
    number_of_schema=${number_of_schema} bash make_graph.sh 
done
