var HomePage = function () {
    var newTaskInput = by.css('.new-task'),
        taskRemoveButton = by.css('.remove button'),
        taskCompleteCheckbox = by.css('.complete input');


    this.refresh = function () {
        browser.get('http://127.0.0.1:8000');
    };

    this.addTask = function (title) {
        element(newTaskInput).sendKeys(title + '\n');
    };

    this.getTaskElement = function (title) {
        return element(by.cssContainingText('.tasks .title', title))
            .element(by.xpath('..'))
    };

    this.removeTask = function (title) {
        this.getTaskElement(title)
            .element(taskRemoveButton)
            .click();
    };

    this.markAsComplete = function (title) {
        this.getTaskElement(title)
            .element(taskCompleteCheckbox)
            .click();
    };

    this.isComplete = function (title) {
        return this.getTaskElement(title)
            .element(taskCompleteCheckbox)
            .getAttribute('checked').then(function (checked) {
                return !!checked;
            });
    };

    this.taskTitles = function () {
        return element.all(by.css('.tasks .title')).getText();
    };
};

describe('TODO list on the home page', function () {
    var homePage;

    beforeEach(function (done) {
        require('request').post('http://127.0.0.1:8000/todo/cleanup', {proxy: ''}, done);
        homePage = new HomePage();
        homePage.refresh();
    });

    it('should allow adding a new task', function () {
        homePage.addTask('First task');

        homePage.refresh();
        expect(homePage.taskTitles()).toEqual(['First task']);
    });

    it('should order tasks by creation time', function () {
        homePage.addTask('First task');
        homePage.addTask('Second task');

        homePage.refresh();
        expect(homePage.taskTitles()).toEqual(['First task', 'Second task']);
    });

    it('should not add new task with empty title', function () {
        homePage.addTask('');

        homePage.refresh();
        expect(homePage.taskTitles()).toEqual([]);
    });

    it('should allow deleting a task', function () {
        homePage.addTask('Task');

        homePage.removeTask('Task');

        homePage.refresh();
        expect(homePage.taskTitles()).toEqual([]);
    });

    it('should delete only requested task', function () {
        homePage.addTask('First task');
        homePage.addTask('Second task');

        homePage.removeTask('First task');

        homePage.refresh();
        expect(homePage.taskTitles()).toEqual(['Second task']);
    });

    it('should not mark new task as complete', function () {
        homePage.addTask('Task');

        homePage.refresh();
        expect(homePage.isComplete('Task')).toBe(false);
    });

    it('should allow marking a task as complete', function () {
        homePage.addTask('Task');

        homePage.markAsComplete('Task');

        homePage.refresh();
        expect(homePage.isComplete('Task')).toBe(true);
    });

    it('should not mark not completed tasks', function () {
        homePage.addTask('Task');

        homePage.refresh();
        expect(homePage.getTaskElement('Task').getAttribute('class')).not.toContain('success');
    });

    it('should mark completed tasks', function () {
        homePage.addTask('Task');
        homePage.markAsComplete('Task');

        homePage.refresh();
        expect(homePage.getTaskElement('Task').getAttribute('class')).toContain('success');
    });
});