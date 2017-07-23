
        ╔══════════════════════════╗
        ║       FreePanorama       ║
        ╚══════════════════════════╝

	PROGRAM
	~~~~~~~
FreePanorama is a free and open-source viewer for panorama files.


	FORMAT
	~~~~~~
	.pan file format is based on uncompressed ZIP[1]
and uncompressed TIFF[2,3]. If you want to create .pan file,
you can get 6 tiff-files:
	b.tif - Back picture
	d.tif - Down picture
	f.tif - Front picture
	l.tif - Left picture
	r.tif - Right picture
	u.tif - Up picture
Then compress these files into zip-archive without compression 
(option "store"). For example, if you use FAR, you can select
this option in compression method:


   ╔═══════════════════════════ Create archive ═══════════════════════════╗
   ║ Profile:                             ↓ [ Save ] [ Delete ]           ║
   ╟──────────────────────────────────────────────────────────────────────╢
   ║ Archive path: [ Evaluate ] [ ] Append extension                      ║
   ║ l439.zip                                                           ↓ ║
   ╟──────────────────────────────────────────────────────────────────────╢
   ║ Select archiver: ( ) 7z (•) zip ( ) Other:bzip2    ↓                 ║
   ║ Compression level:Store      ↓  Method:LZMA     ↓  [x] Solid         ║
   ║ Advanced parameter┌───────────────────┐                            ↓ ║
   ╟───────────────────│  Store            │──────────────────────────────╢
   ║ [ ] Encrypt archiv│  Fastest          │  [ ] Show password           ║
   ║ Password:         │  Fast             │                              ║
   ╟───────────────────│  Normal           │──────────────────────────────╢
   ║ [ ] Create self-ex│  Maximum          │FX options ]                  ║
   ╟───────────────────│  Ultra            │──────────────────────────────╢
   ║ [ ] Split into vol└───────────────────┘                ↓             ║
   ╟──────────────────────────────────────────────────────────────────────╢
   ║ [ ] Delete files after archiving  [ ] Ignore errors                  ║
   ║ [x] Archive files open for writing  [ ] Use filter                   ║
   ╟──────────────────────────────────────────────────────────────────────╢
   ║                 { OK } [ Cancel ] [ Save settings ]                  ║
   ╚══════════════════════════════════════════════════════════════════════╝


And then rename "filename.zip" to "filename.pan".


	EXAMPLE
	~~~~~~~
Here is an example of one pan-file:

Size		Data			Comment

		<--- FILES --->

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"b.tif"			Name of file (back)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		3D 49 12 43		CRC (ignored)
4 bytes		08 02 C0 00		Compressed size (=N)
4 bytes		08 02 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"d.tif"			Name of file (down)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"f.tif"			Name of file (front)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"l.tif"			Name of file (left)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"r.tif"			Name of file (right)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

4 bytes		50 4B 03 04		ZIP header
2 bytes		0A 00			Version need to extract (ignored)
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
5 bytes		"u.tif"			Name of file (up)
X bytes		?? ??			Extra field (ignored)
N bytes		** ** 			TIFF file data

		<--- CENTRAL DIRECTORY RECORDS --->

4 bytes		50 4B 01 02		ZIP header (ignored all below)
2 bytes		3F 00			Version made by 
2 bytes		0A 00			Version need to extract
2 bytes		00 00			Bit Flag (must be ZERO)
2 bytes		00 00			No compression (must be ZERO)
2 bytes		EA 89 			Modification time (ignored)
2 bytes		D3 42			Modification date (ignored)
4 bytes		18 EA A0 B6		CRC (ignored)
4 bytes		80 08 C0 00		Compressed size (=N)
4 bytes		80 08 C0 00		Uncompressed size (=N)
2 bytes		05 00			Size of file name (=5)
2 bytes		00 00			Size of extra fild (=X)
2 bytes		00 00			Size of file comment (=Y)
5 bytes		"b.tif"			Name of file (back)
X bytes		?? ??			Extra field
Y bytes		?? ??			Comment

...


		<--- END OF CENTRAL DIRECTORY --->

4 bytes		50 4B 05 06		ZIP header (ignored all below)
2 bytes		00 00			Number of this disk
2 bytes		00 00			Number of disk where central directory starts
2 bytes		06 00			Number of central directory records on the disk
2 bytes		00 00			Total number of central directory records
4 bytes		00 00 00 00		Size of central directory in bytes
4 bytes		00 00 00 00		Offset of start of central directory (from start of archive)
2 bytes		00 00			Size of comment (=Y)
Y bytes		?? ??			Comment


	LITERATURE
	~~~~~~~~~~

[1] https://en.wikipedia.org/wiki/Zip_(file_format)
[2] https://en.wikipedia.org/wiki/TIFF
[3] https://web.archive.org/web/20091223030231/http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf	
