The Web compiler is built by the following procedure.

1. Generate a makefile
Search all folders under the root and specify it as an include path.
Specify all source files (.c, .cpp) below the root as objects (.o).
Specify all the libraries (.a) below the root as link targets.

2. Run make
Make with the generated makefile. As a result, a bin file for flashing is generated.
Objects(.o) under gr_common folder are archived as gr_common.a.
If gr_common.a already exists, compile and link only sources other than gr_common.

Notes:
· When adding sources and libraries, do not place them in the gr_build or gr_common folder.
  If it is placed elsewhere, include path and object are specified in 1 of the above procedure.
· When editing the library in gr_common, editing is enabled by deleting gr_common.a.
· Since the makefile at the root is automatically generated, it will be invalid even if you edit it.

Webコンパイラのビルドは以下の手順で行われます。

1.makefileを生成
　ルート以下全てのフォルダを検索してインクルードパスとして指定する。
　ルート以下全てのソースファイル(.c, .cpp)をオブジェクト(.o)として指定する。
　ルート以下全てのライブラリ(.a)をリンク対象として指定する。

2.makeの実行
　生成されたmakefileでmakeする。これによって書き込み用のbinファイルが生成される。
　gr_common以下のオブジェクト(.o)はgr_common.aとしてアーカイブ化する。
　gr_common.aがすでにあれば、gr_common以外のソースだけコンパイルしてリンクする。

注意事項：
・ソースやライブラリを追加する場合、gr_buildやgr_commonフォルダ内には配置しないでください。
　それ以外の場所に配置すれば、上記手順の1にてインクルードパス、オブジェクトの指定が行われます。
・gr_common内のライブラリを編集する場合、gr_common.aを削除することで編集が有効になります。
・ルートにあるmakefileは自動生成されるため、編集しても無効となります。