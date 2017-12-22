## Wiki URL
#  This file contains a bunch of help functions (mostly predicates) that deal
#  with Wikipedia URLs

WIKIPEDIA_URL_PREFIX  = "https://en.wikipedia.org/wiki/"

# Given a url string, returns true if and only if the url is a Wikipedia URL
def is_wiki_url(url):
    return (url.find(WIKIPEDIA_URL_PREFIX) == 0)

# Given a string that is a wiki URL. get the article name
def article_name(url):

    assert (is_wiki_url(url))

    # split the URL by WIKIPEDIA_URL_PREFIX to get path part of URL
    return url.split(WIKIPEDIA_URL_PREFIX)[1]

# Given a url string, returns true if and only if the wiki url points to an
# article
def is_article_url(url):

    assert(is_wiki_url(url))

    # split url into "https://en.wikipedia.org/wiki/" and rest
    a_name = article_name(url)
    assert(len(a_name) != 0)

    # is it a section within the same page or probably other pages ?
    # if the url points to a section in another page, we can safely ignore the
    # url, cos if that page is really important, it will eventually show up
    if '#' in a_name:
        return False

    # this is probably a help/project page?
    if ":" in a_name:
        return False

    # add additional url related tests here

    return True

def is_wiki_url_crawlable(url):

    # if the url has a  "/wiki/*" prefix
    if (not is_wiki_url(url)):
        return False

    # does the url point to an article
    # note that, while this function may return true, it does not guarantee!
    # we can know for sure only after page download
    if (not is_article_url(url)):
        return False

    return True
