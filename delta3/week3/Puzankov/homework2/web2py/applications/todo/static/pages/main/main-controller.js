angular.module('todo')
    .controller('MainController', function ($scope, $resource) {
        var Task = $resource('/todo/api/tasks/:id', {id: '@id'});

        function refresh() {
            Task.query(function (tasks) {
                $scope.tasks = tasks;
            });
        }

        refresh();

        $scope.add = function () {
            if ($scope.newTask) {
                var task = new Task({
                    complete: false,
                    title: $scope.newTask
                });
                task.$save()
                    .then(refresh);
                $scope.newTask = undefined;
            }
        };

        $scope.remove = function (task) {
            task.$remove()
                .then(refresh);
        };
    });