/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
class UserFilms {
  final int? id;
  final int userId;
  final int filmId;
  final int choice;

  UserFilms(
      {this.id,
      required this.userId,
      required this.filmId,
      required this.choice});

  UserFilms.fromMap(Map<String, dynamic> map)
      : id = map['id'] as int?,
        userId = map['user_id'] as int,
        filmId = map['film_id'] as int,
        choice = map['choice'] as int;

  Map<String, dynamic> toMap() {
    return {
      'user_id': userId,
      'film_id': filmId,
      'choice': choice,
    };
  }

  @override
  String toString() {
    return 'UserFilms{id: $id, user_id: $userId, film_id: $filmId, choice: $choice}';
  }
}
