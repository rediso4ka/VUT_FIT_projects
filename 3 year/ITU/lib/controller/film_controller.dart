/// Authors: 
/// Vadim Goncearenco (xgonce00@stud.fit.vutbr.cz)
/// 

import 'package:flutter/material.dart';
import 'package:sqflite/sqflite.dart';

import 'package:itu_app/model/film_model.dart';
import 'package:itu_app/model/db_helper.dart';

class FilmController extends ChangeNotifier {
  final DatabaseHelper _databaseHelper = DatabaseHelper();
  
  Future<void> insertFilm(Film film) async {
    final db = await _databaseHelper.database;
    await db.insert('films', 
      film.toMap(),
      conflictAlgorithm: ConflictAlgorithm.replace);
    notifyListeners();
  }

  Future<List<Film>> films() async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps = await db.query('films', orderBy: 'id');
    return List.generate(maps.length, (i) {
      return Film.fromMap(maps[i]);
    });
  }

  Future<Film> film(int id) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps = await db.query('films', where: 'id = ?', whereArgs: [id], limit: 1);
    return Film.fromMap(maps[0]);
  }
  
  Future<void> updateFilm(int id) async {
    final db = await _databaseHelper.database;
    final film = await this.film(id);

    await db.update('films', film.toMap(), where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> deleteFilm(int id) async {
    final db = await _databaseHelper.database;
    await db.delete('films', where: 'id = ?', whereArgs: [id]);
    notifyListeners();
  }

  Future<void> clearFilms() async {
    final db = await _databaseHelper.database;
    await db.delete('films');
    notifyListeners();
  }

  Future<void> clearDatabase() async {
    await _databaseHelper.clearDatabase();
    notifyListeners();
  }

  Future<List<Film>> getUnseenFilms(int userId) async {
    final db = await _databaseHelper.database;
    final List<Map<String, dynamic>> maps = await db.rawQuery('''
      SELECT * FROM films WHERE id NOT IN (
        SELECT film_id FROM user_films WHERE user_id = $userId
      )
    ''');
    return List.generate(maps.length, (i) {
      return Film.fromMap(maps[i]);
    });
  }
}
