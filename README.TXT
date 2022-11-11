●libjeg
 -JPEGイメージ画像関連のライブラリー
 -ダウンロード：http://www.ijg.org/
 -MAKE　（Windows）:
　　１．jpegsrc.v6b.tar.gz　をダウンロード。
　　２．READMEを参照
　　３．install.docを参照
　　４．ファイルをリネーム。　jconfig.vc → jconfig.h, makelib.ds → jpeg.mak
　　５．jpeg.makをMSDEVで開きコンパイル。
●libpng
 -PNGイメージ画像関連のライブラリー
 -ダウンロード：http://www.libpng.org/pub/png/pngcode.html
 -zlibが必要
 -MAKE　（Windows）:
　　projects\visualc6\libpng.dsw を開いてコンパイル。
　　すればいいのだが、改行コードがLFなので、このディレクトリ
　　にあるファイルをFTPなどで転送して、CRLFに変換すること！！
●libtiff
 -TIFF イメージ画像関連のライブラリー
 -ダウンロード：http://www.libtiff.org/
 -MAKE　（Windows）:
　　１．コマンドラインで、.\tiff-3.7.2に移動
　　２．VCVARS32.BAT　を実行し環境変数登録
　　３．nmake -f Makefile.vc　を実行しLIBを作成
●libungif
 -GIFイメージ画像関連のライブラリー
 -ダウンロード：http://ftp.yz.yamagata-u.ac.jp/pub/cygwin/release/libungif/

●zlib
 -gzip関連のアーカイブライブラリー
 -ダウンロード：http://www.gzip.org/zlib/
 -MAKE　（Windows）:
　　projects\visualc6\zlib.dsw をオープンしてコンパイルすればOK



