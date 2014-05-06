#!/usr/bin/perl

if(($#ARGV + 1)  < 3){
	print( "USAGE : $0 input_dir input_image_extension output_dir \n" );
	exit;
}

my $input_dir = $ARGV[0];
my $img_ext = $ARGV[1];
my $output_dir = $ARGV[2];

if(!(-d $output_dir)){
	mkdir ($output_dir) || die "Error creating output dir: $output_dir";
}

opendir ( DIR, $input_dir ) || die "Error in opening input dir: $input_dir\n";
while( ($filename = readdir(DIR))){
    if($filename =~ /$img_ext$/){
	my $output_filename = $filename;
	$output_filename =~ s/$img_ext$/tiff/;
	print("$input_dir/$filename --> $output_dir/$output_filename\n");
	system("dcraw -v -4 -H 0 -W -r 1.308594 1.000000 1.949219 1.000000 -q 3 -T -o 0 $input_dir/$filename; mv $input_dir/$output_filename $output_dir");
	if($filename =~ /^07_C_d/){
		my $jpeg_filename = $filename;
		$jpeg_filename =~ s/$img_ext$/jpg/;
		system("convert $output_dir/$output_filename $output_dir/$jpeg_filename");
	}
    }
}
closedir(DIR);
