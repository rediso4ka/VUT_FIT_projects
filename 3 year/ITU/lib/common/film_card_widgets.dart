/// Authors:
/// Vadim Goncearenco (xgonce00@stud.fit.vutbr.cz)
///
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';

class CommentsWidget extends StatelessWidget {
  final int filmId;

  const CommentsWidget({Key? key, required this.filmId}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 50,
      child: TextButton(
        onPressed: () {
          context.push('/listFilms/$filmId/comments');
        },
        child: const Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.comment),
            SizedBox(width: 8),
            Text('View Comments'),
          ],
        ),
      ),
    );
  }
}

class FilmAttribute extends StatelessWidget {
  final String attribName;
  final String attribValue;

  const FilmAttribute(
      {Key? key, required this.attribName, required this.attribValue})
      : super(key: key);

  @override
  Widget build(BuildContext context) {
    return RichText(
      text: TextSpan(
        children: [
          TextSpan(
            text: attribName,
            style: const TextStyle(
              fontWeight: FontWeight.bold,
              color: Colors.black,
              fontSize: 18,
            ),
          ),
          TextSpan(
            text: attribValue,
            style: const TextStyle(
              fontWeight: FontWeight.normal,
              color: Colors.black,
              fontSize: 16,
            ),
          ),
        ],
      ),
    );
  }
}

class FilmPoster extends StatelessWidget {
  final String posterPath;

  const FilmPoster({Key? key, required this.posterPath}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return (posterPath != "<No poster>"
        ? Container(
            height: 420,
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(20),
              image: DecorationImage(
                image: Image.file(File(posterPath)).image,
                fit: BoxFit.fill,
              ),
            ),
          )
        : const SizedBox(
            height: 200,
            child: Text("<No poster>"),
          ));
  }
}
