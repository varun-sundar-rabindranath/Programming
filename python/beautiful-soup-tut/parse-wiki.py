from bs4 import Tag, BeautifulSoup
import requests

from wiki_url import *

# Wikipedia domain name
WIKI_DOMAIN_NAME = "https://en.wikipedia.org"

EOD_MARKERS = ["See_also", "References", "External_links"]

# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if the tag appears in one of the headings in
# EOD_MARKERS
def tag_in_eod_marker(tag):
    # have a h2 tag
    for span_tag in (tag.find_all("span", { 'class' : 'mw-headline' })):
        if (span_tag.get('id') in EOD_MARKERS):
            return True
    return False


# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if the tag appears as a part of a paragraph
def tag_in_p(tag):
    return (tag.find_parents('p') != [])

# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if the tag appears as a part of a table
def tag_in_table(tag):
    return (tag.find_parents("table") != [])

# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if the tag appears as a part of a wikitable
def tag_in_wikitable(tag):
    return (tag.find_parents("table", { 'class' : 'wikitable'}) != [])

# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if the tag appears in a <li> tag
def tag_in_li(t):
    return (t.find_parents("li") != [])

# Given a BeautifulSoup tag parsed from a Wikipedia article
# returns true if and only if tag is a end of document heading
def tag_is_eod_heading(tag):

    # End of document is determined based on Wikipedia headings
    if (tag.name != "h2"):
        return False

    # Check if we are in a end of document heading
    return tag_in_eod_marker(tag)

# Given all the direct children, as a list of tags, of the "mw-parser-output"
# div (which essentially contains the main article content, along with other
# navigational links), returns ONLY those direct children that actually make up
# the document
def in_doc_tags(tags):

    rel_tag = []

    for tag in tags:
        if tag_is_eod_heading(tag):
            # reached our end of document marker.
            break;
        else:
            rel_tag.append(tag)

    return rel_tag

# Given a list of tags, where each element in the list  could be eiter
# NavigableString or Tag,
# returns all the anchor tags found within each tag in the input list of tags
def all_children_anchors(tags):

    anchors = []

    for tag in tags:
        if (isinstance(tag, Tag)):
            anchors = anchors + tag.find_all('a')

    return anchors

# Given a list of anchor tags
# returns a subset of the input list, that pass the following conditions
#   i. The anchor tag appears inside a paragraph tag
#  ii. The anchor tag appears inside a wikitable
# iii. The anchor tag appears in a list (<li> tag) inside the article content
def topical_anchors (anchors):

    ta = []

    paragraph_anchor_encountered = False

    for anchor in anchors:

        # check if some paragraph anchor is seen.
        paragraph_anchor_encountered = paragraph_anchor_encountered or \
                                       tag_in_p(anchor)

        if anchor in ta:
            continue

        if anchor.get("href") is None:
            continue

        # is the anchor even crawlable?
        if not is_wiki_url_crawlable(WIKI_DOMAIN_NAME + anchor.get("href")):
            continue

        keep = False

        keep = keep or tag_in_p(anchor)
        keep = keep or tag_in_wikitable(anchor)
        # the idea is that if we have encountered the article paragraph
        # then we assume that the list anchors that we encounter after is
        # related to the document
        keep = keep or (paragraph_anchor_encountered and \
                        tag_in_li(anchor))
        # dont consider anchors in any table that is not a wikitable
        # if tag appears in a table, it MUST be wiki table
        keep = keep and ((not tag_in_table(anchor)) or \
                          tag_in_wikitable(anchor))

        if keep:
            ta.append(anchor)

    return ta

r = requests.get("https://en.wikipedia.org/wiki/Tropical_fish").content

soup = BeautifulSoup(r, 'html.parser')

content = soup.find("div", { 'class' : 'mw-parser-output' })

rel_tags = in_doc_tags(content.contents)


anchors = get_anchors(rel_tags)

#print anchors

a_anchors = topical_anchors(anchors)


for anchor in a_anchors:
    print "anchor - ", anchor.text
