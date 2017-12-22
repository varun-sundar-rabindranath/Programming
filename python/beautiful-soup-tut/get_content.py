import requests
from bs4 import Tag, BeautifulSoup
import sys

reload(sys)
sys.setdefaultencoding('utf-8')

# End of document markers
# Wikipedia articles generally end with these headings. We consider the
# document to have ended when we encounter these headings in a page.
# headings are put in span tag of class "mw-headline" and with a descriptive
# ID. We record the IDs here
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

def is_p_tag(t):
    return (t.name == "p")

def is_headline_tag(t):
    # is there a span tag? that contains a headline class????
    return (t.find_all("span", { 'class' : 'mw-headline' }) != [])

def is_firstheading_tag(t):
    # does the tag contain a firstheading class
    if t.has_attr('class') and t.has_attr('id'):
        return t['class'] == 'firstHeading' and t['id'] == 'firstHeading'
    return False

def is_ul_tag(t):
    return (t.name == "ul")

def is_ol_tag(t):
    return (t.name == "ol")

def p_or_headline(t):
    return is_p_tag(t) or is_headline_tag(t) or is_firstheading_tag(t)

def contains_sup(t):
    return (t.find_all("sup") != [])

def extract_all_sup(t):
    while contains_sup(t):
        t.sup.extract()

def print_p(ptag):
    assert is_p_tag(ptag)
    extract_all_sup(ptag)
    print t.text

def print_headline(htag):
    assert is_headline_tag(htag)
    print htag.find("span", { 'class' : 'mw-headline' }).text

def print_summary(text):
    return text[0: 20].strip()

# String -> String
# Given a path to a Wikipedia document
# returns the html content of the document
def doc_html(docpath):

    # get doc
    doc = open(docpath)
    assert(doc is not None)

    # first line is the URL
    url = doc.readline()

    # rest of the lines is the HTML
    html = doc.read()

    doc.close()

    return html


# Get html doc
#rc = requests.get('https://en.wikipedia.org/wiki/URL_shortening').content

html =  doc_html('/home/varun/Documents/NEU-COURSES/IR/ir/test/100-year_flood.html');

# soup it
soup = BeautifulSoup(html, 'html.parser')

# get the content div
content_soup = soup.find("div", { 'class' : 'mw-parser-output'})

# content tags
content_tags = content_soup.contents #content_soup.find_all(p_or_headline)


h1_t = soup.find_all("h1")

#print h1_t

for t in content_tags:

    if (not isinstance(t, Tag)) or (t.text is None):
        continue

    if tag_in_eod_marker(t):
        # stop printing
        break

    if is_p_tag(t):
        print_p(t)
        print "\n"

    elif is_headline_tag(t):
        print "\n"
        print_headline(t)
        print "\n"

    elif is_ul_tag(t):
        print t.text

    elif is_ol_tag(t):
        print t.text

#for child in content_soup.children:
#
#    if (not isinstance(child, Tag)) or (child.text is None):
#        continue
#
#    if child in content_tags:
#        print "Tag Accounted !! - ", print_summary(child.text)
#    else:
#        print "Tag UnAccounted !! - ", print_summary(child.text)
