from pytest import mark
from wc_utils import count_bytes, count_chars, count_lines, count_words

@mark.parametrize('data, expected', [
    (b'hello\nfrom\nstring\n', 3), 
    (b'', 0), 
    (b'\n\n', 2),
    (b'\r\n\r\n', 2)])
def test_count_lines(data, expected):
    assert count_lines(data) == expected

@mark.parametrize('data, expected', [
    (b'hello world', 2),
    (b'   hello   world   with spaces', 4),
    (b'', 0),
    (b'hello, world', 2)])
def test_count_words(data, expected):
    assert count_words(data) == expected

@mark.parametrize('data, expected', [
    (b'\xF0\x9F\x98\x81', 1),
    (b'\xF0\x9F\x98\x81\xF0\x9F\x98\x82\xF0\x9F\x98\x81', 3),
    (b'abc\xF0\x9F\x98\x81', 4),
    (b'', 0)])
def test_count_chars(data, expected):
    assert count_chars(data) == expected

@mark.parametrize('data, expected', [
    (b'\xF0\x9F\x98\x81', 4),
    (b'\xF0\x9F\x98\x81\xF0\x9F\x98\x82\xF0\x9F\x98\x81', 12),
    (b'abc\xF0\x9F\x98\x81', 7),
    (b'', 0)])
def test_count_bytes(data, expected):
    assert count_bytes(data) == expected