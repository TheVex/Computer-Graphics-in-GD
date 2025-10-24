#include "raytracer_renderer.h"

#include "utils/resource_utils.h"
#include "utils/timer.h"

#include <iostream>


void cg::renderer::ray_tracing_renderer::init()
{

	renderer::load_model();
	renderer::load_camera();
	
	render_target =  std::make_shared<cg::resource<cg::unsigned_color>>(settings->width, settings->height);

	raytracer = std::make_shared<cg::renderer::raytracer<cg::vertex, cg::unsigned_color>>();

	raytracer->set_render_target(render_target);
	raytracer->set_viewport(settings->width, settings->height);
	raytracer->set_vertex_buffers(model->get_vertex_buffers());
	raytracer->set_index_buffers(model->get_index_buffers());


	// TODO Lab: 2.03 Add light information to `lights` array of `ray_tracing_renderer`
	// TODO Lab: 2.04 Initialize `shadow_raytracer` in `ray_tracing_renderer`
}

void cg::renderer::ray_tracing_renderer::destroy() {}

void cg::renderer::ray_tracing_renderer::update() {}

void cg::renderer::ray_tracing_renderer::render()
{
	// TODO Lab: 2.01 Implement `miss_shader`, image clearing, calling `ray_generation`, and saving in `ray_tracing_renderer` class
	raytracer->clear_render_target({0, 0, 0});

	raytracer->miss_shader = [](const ray& ray) {
		payload payload{};
		payload.color = {0.f, 0.f, (ray.direction.y + 1.f) * 0.5f};
		return payload;
	};

	{
		cg::utils::timer t("Ray generation");

		raytracer->ray_generation(
			camera->get_position(), camera->get_direction(),
			camera->get_right(), camera->get_up(),
			settings->raytracing_depth, settings->accumulation_num
		);
	}

	cg::utils::save_resource(*render_target, settings->result_path);

	// TODO Lab: 2.02 Add `closest_hit_shader` to `raytracer` class to return diffuse color
	// TODO Lab: 2.03 Adjust `closest_hit_shader` of `raytracer` to implement Lambertian shading model
	// TODO Lab: 2.04 Define `any_hit_shader` and `miss_shader` for `shadow_raytracer`
	// TODO Lab: 2.04 Adjust `closest_hit_shader` of `raytracer` to cast shadows rays and to ignore occluded lights
	// TODO Lab: 2.05 Adjust `ray_tracing_renderer` class to build the acceleration structure
	// TODO Lab: 2.06 (Bonus) Adjust `closest_hit_shader` for Monte-Carlo light tracing
}