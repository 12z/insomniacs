routes_in = [
    ('/', '/todo/static/index.html'),
    ('/admin/$anything', '/admin/$anything'),
    ('/todo/appadmin/$anything', '/todo/appadmin/$anything'),
    ('/todo/api/$anything', '/todo/api/$anything'),
    ('/todo/cleanup', '/todo/cleanup'),
    ('/$anything', '/todo/static/$anything')
]