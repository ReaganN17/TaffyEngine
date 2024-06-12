#define isdown(b) input->buttons[b].down
#define pressed(b) (input->buttons[b].down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].down && input->buttons[b].changed)

float player_x = 0, player_y = 0, speed = 500;
float alpha = 1, wscale = 1, hscale = 1;
u32 color = 0;

Image test("resources/bob.png");

internal void simulateInit() {
	
}

internal void simulate(Input* input, float dt) {
	renderCoolBG();

	if(isdown(LEFT)) player_x -= speed * dt;
	if (isdown(RIGHT)) player_x += speed * dt;
	if (isdown(UP)) player_y += speed * dt;
	if (isdown(DOWN)) player_y -= speed * dt;

	if (isdown(S)) hscale *= 1.01f;
	if (isdown(W)) hscale *= 0.99f;

	if (isdown(A)) wscale *= 1.01f;
	if (isdown(D)) wscale *= 0.99f;


	hscale = clampF(0, hscale, hscale);
	wscale = clampF(0, wscale, wscale);
	alpha = clampF(0, alpha, 1);
	color %= 100;

	drawImageFinal(test, player_x, player_y, wscale, hscale, alpha);

}

