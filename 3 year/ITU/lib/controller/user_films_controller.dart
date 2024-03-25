/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
import 'package:flutter/material.dart';
import 'package:itu_app/model/db_helper.dart';
import 'package:itu_app/model/user_films_model.dart';
import 'package:sqflite/sqflite.dart';

class UserFilmsController extends ChangeNotifier {
  final DatabaseHelper _databaseHelper = DatabaseHelper();

  Future<List<UserFilms>> userFilms(int userId) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps =
        await db.query('user_films', where: 'user_id = ?', whereArgs: [userId]);
    return List.generate(maps.length, (i) {
      return UserFilms.fromMap(maps[i]);
    });
  }

  Future<UserFilms> userFilm(int id) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps = await db.query('user_films',
        where: 'id = ?', whereArgs: [id], limit: 1);
    return UserFilms.fromMap(maps[0]);
  }

  Future<void> updateUserFilm(int id) async {
    final db = await _databaseHelper.database;
    final userFilm = await this.userFilm(id);

    await db.update('user_films', userFilm.toMap(),
        where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> deleteUserFilm(int id) async {
    final db = await _databaseHelper.database;
    await db.delete('user_films', where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> clearUserFilms() async {
    final db = await _databaseHelper.database;
    await db.delete('user_films');
    notifyListeners();
  }

  Future<void> addUserFilm(int userId, int filmId, bool liked) async {
    final db = await _databaseHelper.database;

    final existingUserFilm = await db.query('user_films',
        where: 'user_id = ? AND film_id = ?', whereArgs: [userId, filmId]);

    if (existingUserFilm.isEmpty) {
      await db.insert(
        'user_films',
        UserFilms(
          userId: userId,
          filmId: filmId,
          choice: liked ? 1 : 0,
        ).toMap(),
        conflictAlgorithm: ConflictAlgorithm.replace,
      );
      notifyListeners();
    }
  }

  Future<void> deleteAllUserFilms(int userId) async {
    final db = await _databaseHelper.database;
    await db.delete('user_films', where: 'user_id = ?', whereArgs: [userId]);
    notifyListeners();
    print('row in user_films deleted!\n\n');
  }
  
}
