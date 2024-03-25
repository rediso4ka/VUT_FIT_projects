/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
class Comment {
  final int? id;
  final int filmId;
  final String text;
  final String timestamp;

  Comment({
    this.id,
    required this.filmId,
    required this.text,
    required this.timestamp,
  });

  Comment.fromMap(Map<String, dynamic> map)
      : id = map['id'] as int?,
        filmId = map['film_id'] as int,
        text = map['text'] as String,
        timestamp = map['timestamp'] as String;

  Map<String, dynamic> toMap() {
    final map = {
      'film_id': filmId,
      'text': text,
      'timestamp': timestamp,
    };

    if (id != null) {
      map['id'] = id!;
    }

    return map;
  }

  @override
  String toString() {
    return 'Comment{id: $id, filmId: $filmId, text: $text}';
  }
}
