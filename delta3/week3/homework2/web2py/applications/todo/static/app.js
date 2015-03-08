angular.module('todo', [
    'ngResource',
    'ngRoute'
])
    .config(function ($routeProvider) {
        $routeProvider
            .when('/', {
                templateUrl: 'pages/main/main.html',
                controller: 'MainController'
            })
            .when('/about', {
                templateUrl: 'pages/about/about.html'
            })
            .otherwise('/')
    });