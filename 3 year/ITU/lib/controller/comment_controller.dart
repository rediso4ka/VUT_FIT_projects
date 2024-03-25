/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
import 'package:flutter/material.dart';
import 'package:sqflite/sqflite.dart';

import 'package:itu_app/model/comment_model.dart';
import 'package:itu_app/model/db_helper.dart';

class CommentController extends ChangeNotifier {
  final DatabaseHelper _databaseHelper = DatabaseHelper();

  Future<void> insertComment(Comment comment) async {
    final db = await _databaseHelper.database;
    await db.insert(
      'comments',
      comment.toMap(),
      conflictAlgorithm: ConflictAlgorithm.replace,
    );
    notifyListeners();
  }

  Future<List<Comment>> comments(int filmId) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps =
        await db.query('comments', where: 'film_id = ?', whereArgs: [filmId]);
    return List.generate(maps.length, (i) {
      return Comment.fromMap(maps[i]);
    });
  }

  Future<Comment> comment(int id) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps =
        await db.query('comments', where: 'id = ?', whereArgs: [id], limit: 1);
    return Comment.fromMap(maps[0]);
  }

  Future<void> updateComment(int id) async {
    final db = await _databaseHelper.database;
    final comment = await this.comment(id);

    await db.update('comments', comment.toMap(), where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> deleteComment(int id) async {
    final db = await _databaseHelper.database;
    await db.delete('comments', where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> clearComments() async {
    final db = await _databaseHelper.database;
    await db.delete('comments');
    notifyListeners();
  }
}
