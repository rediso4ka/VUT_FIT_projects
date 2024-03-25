/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
import 'package:flutter/material.dart';

import 'package:provider/provider.dart';

import 'package:itu_app/model/comment_model.dart';
import 'package:itu_app/controller/comment_controller.dart';

class CommentsScreen extends StatefulWidget {
  final int filmIndex;

  const CommentsScreen({required this.filmIndex, Key? key}) : super(key: key);

  @override
  State<CommentsScreen> createState() => _CommentsScreenState();
}

class _CommentsScreenState extends State<CommentsScreen> {
  final TextEditingController _commentTextController = TextEditingController();

  @override
  Widget build(BuildContext context) {
    var filmComments =
        context.watch<CommentController>().comments(widget.filmIndex);

    return Scaffold(
      appBar: AppBar(
        title: const Text('Comments'),
        backgroundColor: const Color.fromARGB(255, 68, 70, 115),
        foregroundColor: Colors.white,
      ),
      body: Column(
        children: [
          Expanded(
            child: FutureBuilder<List<Comment>>(
              future: filmComments,
              builder: (context, snapshot) {
                if (snapshot.connectionState == ConnectionState.waiting) {
                  return const CircularProgressIndicator();
                } else if (snapshot.hasError) {
                  return Text('Error: ${snapshot.error}');
                } else {
                  List<Comment> comments = snapshot.data ?? [];
                  return ListView.builder(
                    itemCount: comments.length,
                    itemBuilder: (context, index) {
                      return ListTile(
                        title: Text(comments[index].text),
                        subtitle: Text(comments[index].timestamp),
                      );
                    },
                  );
                }
              },
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: _commentTextController,
                    decoration:
                        const InputDecoration(hintText: 'Add a comment'),
                  ),
                ),
                IconButton(
                  icon: const Icon(Icons.send),
                  onPressed: () {
                    var commentController = context.read<CommentController>();
                    commentController.insertComment(
                      Comment(
                        filmId: widget.filmIndex,
                        text: _commentTextController.text,
                        timestamp: DateTime.now().toString(),
                      ),
                    );
                    _commentTextController.clear();
                  },
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }

  @override
  void dispose() {
    _commentTextController.dispose();
    super.dispose();
  }
}
