/// Authors:
/// Vadim Goncearenco (xgonce00@stud.fit.vutbr.cz)
///

import 'package:flutter/foundation.dart';
import 'package:sqflite/sqflite.dart';
import 'package:path/path.dart';

class DatabaseHelper extends ChangeNotifier {
  static final DatabaseHelper _instance = DatabaseHelper._privateConstructor();
  static Database? _database;

  static const String _databaseName = 'film_database.db';

  DatabaseHelper._privateConstructor();

  factory DatabaseHelper() {
    return _instance;
  }

  Future<Database> get database async {
    if (_database != null) {
      return _database!;
    }

    _database = await _initDatabase();
    return _database!;
  }

  Future<Database> _initDatabase() async {
    final path = join(await getDatabasesPath(), _databaseName);
    return openDatabase(path, version: 2, onCreate: _onCreate);
  }

  Future<void> _onCreate(Database db, int version) async {
    print('CREATING DATABASE\n\n\n\n\n\n\n');

    await db.execute('''
      CREATE TABLE films (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        title TEXT NOT NULL,
        releaseDate TEXT NOT NULL,
        duration TEXT,
        director TEXT,
        overview TEXT,
        actors TEXT,
        posterPath TEXT,
        UNIQUE(title, releaseDate)
      )
    ''');

    await db.execute('''
      CREATE TABLE comments (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        film_id INTEGER NOT NULL,
        text TEXT NOT NULL,
        timestamp TEXT NOT NULL,
        FOREIGN KEY (film_id) REFERENCES films (id)
      )''');

    await db.execute('''
      CREATE TABLE users (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        name TEXT NOT NULL
      )
    ''');

    await db.execute('''
      CREATE TABLE user_films (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        user_id INTEGER NOT NULL,
        film_id INTEGER NOT NULL,
        choice INTEGER NOT NULL,
        FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE,
        FOREIGN KEY (film_id) REFERENCES films (id) ON DELETE CASCADE
      )
    ''');
  }

  Future<void> clearDatabase() async {
    try {
      final databaseFile = join(await getDatabasesPath(),
          _databaseName);

      await deleteDatabase(databaseFile);
      print('DATABASE DELETED\n\n\n');
    } catch (e) {
      print('ERROR DELETING DATABASE: $e\n\n\n');
    }
  }
}
