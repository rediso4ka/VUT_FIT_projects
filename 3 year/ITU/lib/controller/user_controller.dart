/// Authors:
/// Tatiana Fedorova (xfedor14@stud.fit.vutbr.cz)
///
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:itu_app/model/db_helper.dart';
import 'package:sqflite/sqflite.dart';

class UserController extends ChangeNotifier {
  final DatabaseHelper _databaseHelper = DatabaseHelper();

  Future<void> addUser(String name) async {
    final db = await _databaseHelper.database;
    await db.insert(
      'users',
      {'name': name},
      conflictAlgorithm: ConflictAlgorithm.replace,
    );
    print('USER ADDED\n');
  }

  Future<void> removeUser(int id) async {
    final db = await _databaseHelper.database;

    await db.transaction((txn) async {
      await txn.delete(
        'user_films',
        where: 'user_id = ?',
        whereArgs: [id],
      );

      await txn.delete(
        'users',
        where: 'id = ?',
        whereArgs: [id],
      );
    });

    print('USER DELETED\n');
  }

  Future<List<Map<String, dynamic>>> getUsers() async {
    final db = await _databaseHelper.database;
    return await db.query('users');
  }

  Future<String> getUserName(int id) async {
    final db = await _databaseHelper.database;
    final user = await db.query(
      'users',
      where: 'id = ?',
      whereArgs: [id],
    );
    print('USER NAME: ${user[0]['name']}\n');
    return user[0]['name'].toString();
  }
}
