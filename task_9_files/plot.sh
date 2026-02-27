clear && clear

nodes_type=uniform   # uniform | chebyshev | random

python3 plot.py --nodes "${nodes_type}" --show
