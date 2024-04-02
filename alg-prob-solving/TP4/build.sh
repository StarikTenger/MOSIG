g++ -o tp4 main.cpp

./tp4 TREE <input.txt >output_tree.txt
dot -Kfdp -n -Tpng -o graph_tree.png output_tree.txt

./tp4 DFS <input.txt >output_dfs.txt
dot -Kfdp -n -Tpng -o graph_dfs.png output_dfs.txt

./tp4 GREED <input.txt >output_greed.txt
dot -Kfdp -n -Tpng -o graph_greed.png output_greed.txt