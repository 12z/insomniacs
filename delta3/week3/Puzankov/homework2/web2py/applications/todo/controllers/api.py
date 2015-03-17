from gluon.contrib import simplejson
from gluon.serializers import json


def _get_all():
    all_tasks = db().select(db.task.ALL)
    return json(all_tasks)


def _get(task_id):
    task = db.task[task_id]
    return json(task)


def _create(task):
    return db.task.insert(title=task['title'])


def _check():
    task = simplejson.loads(request.body.read())

    task_id = _update(task)

    return _get(task_id)


def _update(task):
    print(task['title'], task['complete'])
    row = db(db.task.title == task['title']).update(complete=task['complete'])
    return row


def _save():
    task = simplejson.loads(request.body.read())

    task_id = _create(task)

    return _get(task_id)


def _delete():
    task_id = request.args[0]
    db(db.task.id == task_id).delete()


def tasks():
    if request.env.request_method == 'PUT':
        return _check()
    elif request.env.request_method == 'POST':
        return _save()
    elif request.env.request_method == 'DELETE':
        return _delete()
    else:
        return _get_all()
