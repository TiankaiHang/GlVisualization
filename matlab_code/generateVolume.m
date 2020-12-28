function data = generateVolume()
    N = 256;
    RADIUS = 20;
    data = zeros(N, N, N);
    [x, y, z] = meshgrid(zeros(N, 1));
    display(x)
    plot3(x);

end