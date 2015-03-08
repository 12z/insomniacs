# -*- coding: utf-8 -*-
'''
Module for text statistics calculation.
'''
import re;
from collections import defaultdict;

def get_char_frequencies(text):
    """
    Count the frequency of each character in the passed text.

    @param text: text to process
    @type text: C{string}

    @return: dictionary containing pairs <character>: <count>
    @rtype: C{dict}
    """
    char_freq = defaultdict(int);
    for k in text:
        char_freq[k] += 1;
    return dict(char_freq);

def split_to_words(text):
    """
    Splits the passed text into words.

    @param text: text to process
    @type text: C{string}

    @return: list containing all words from the text
    @rtype: C{list}
    """
    # used filter to remove empty strings
    return filter(None, re.split("[\W+_]", text, flags = re.U));

def get_word_frequencies(text):
    """
    Count the frequency of each word in the passed text.

    @param text: text to process
    @type text: C{string}

    @return: dictionary containing pairs <word>: <count>
    @rtype: C{dict}
    """
    words = split_to_words(text);
    word_freq = defaultdict(int);
    for k in words:
        word_freq[k] += 1;
    return dict(word_freq);

def main():
    print(get_char_frequencies("abbbcc")) 
    # Expected output: {'a': 1, 'c': 2, 'b': 3}
    
    print(get_word_frequencies("To be, or not to be?")) 
    # Expected: {'not': 1, 'To': 1, 'or': 1, 'to': 1, 'be': 2})

if __name__ == '__main__':
    main()
