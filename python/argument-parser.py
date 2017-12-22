import argparse

argparser = argparse.ArgumentParser(description='''
    Test argparse
    ''')

argparser.add_argument("--seed",
                       metavar='s',
                       default="world",
                       type=str,
                       help="Seed URL to crawl")


args = vars(argparser.parse_args())

print "seed - ", args["seed"]

