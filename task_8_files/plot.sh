clear && clear

set_number=1
nodes_type=uniform # uniform | chebyshev

python3 plot.py --set ${set_number} --nodes ${nodes_type} --show
