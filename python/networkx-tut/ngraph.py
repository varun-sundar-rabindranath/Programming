import argparse
import networkx as nx
import matplotlib.pyplot as plt

argparser = argparse.ArgumentParser()
argparser.add_argument("--file", required = True, type=str)

args = vars(argparser.parse_args())

gfilename = args["file"]

gfile = open(gfilename, "r")

gfile_lines = gfile.read().splitlines()

adj_list = {}

def add_adj_list_entry(al, node):
    if al.get(node) is None:
        al[node] = ()

def make_link(al, rnode, snode):
    assert(al.get(rnode) is not None)
    assert(al.get(snode) is not None)
    al[rnode] = al[rnode] + (snode, )

for line in gfile_lines:

    receiving_node = line.split(" ")[0]

    add_adj_list_entry(adj_list, receiving_node)

    for node in line.split(" "):

        if node != receiving_node:

            sending_node = node

            add_adj_list_entry(adj_list, sending_node)

            make_link(adj_list, receiving_node, sending_node)

g = nx.DiGraph()

for node in adj_list:
    g.add_node(node)

for rnode in adj_list:
     snodes = adj_list[rnode]
     for snode in snodes:
         g.add_edge(snode, rnode)

print g.number_of_nodes()
print g.number_of_edges()

print g.adj

nd_pairs = nx.degree(g)

nodes   = []
degrees = []

for nd_pair in nd_pairs:
    nodes.append(nd_pair[0])
    degrees.append(nd_pair[1])

#plt.subplot(121)
nx.draw_shell(g, nodelist = nodes,
                 node_size = [ d * 500 for d in degrees ],
                 with_labels=True,
                 alpha = 1.0,
                 node_color = 'turquoise',
                 edge_color = 'grey',
                 font_weight='bold')

plt.show()
